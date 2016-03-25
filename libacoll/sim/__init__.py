from path import path
from pyavrutils.arduino import Arduino, ArduinoCompileError
from pysimavr.avr import Avr
from pysimavr.sim import ArduinoSim
import csv
import logging

log = logging.getLogger(__name__)

root = path(__file__).parent.parent.abspath()


def targets():
    return Avr.arduino_targets

TEMPLATE = '''
#include <coll.h>

void setup()
{
    Serial.begin(9600);

    snippet;

}

void loop()
{
}
'''


def simulator(snippet=None, mcu=None, code=None):
    return ArduinoSim(snippet=snippet,
                      code=code,
                      mcu=mcu,
                      #                      extra_lib=root,
                      template=TEMPLATE,
                      timespan=0.1  # 100ms
                      )


def snippet_doc(csvinput, outdir, logger=lambda x: None):
    d = outdir
    f = open(csvinput, 'rb')
    reader = csv.reader(f)

    fx = open(d / 'generated_examples.csv', 'wb')
    writer = csv.writer(fx)
    logger('generating ' + fx.name)

    path(d / 'generated_template.c').write_text(TEMPLATE)

    writer.writerow([
                    'comment',
                    'snippet',
                    ] + targets())

    for i, (snippet, comment) in enumerate(reader):
        snippet = snippet.replace('{LF}', '\n')
        fcode = path(d / 'generated_code_' + str(i) + '.c')

        msg = 'index: %s generating: %s ' % (i, fcode)
        log.debug(msg)
        logger(msg)

        fcode.write_text(snippet)

        outs = []
        for mcu in targets():
            log.debug('simulating output ' + mcu)
            flog = path(d / 'generated_log_' + mcu + '_' + str(i) + '.c')
            flog.write_text(simulator(snippet, mcu).get_serial())
            outs += ['.. literalinclude:: ' + flog.name]
        writer.writerow([
                        comment,
                        '.. literalinclude:: ' + fcode.name,
                        ] + outs)


def libsize(csvinput, outdir, mcu='atmega168', logger=lambda x: None):
    '''calculate lib size'''
    d = outdir
    f = open(csvinput, 'rb')
    reader = csv.reader(f)

    fx = open(d / 'generated_code_sizes.csv', 'wb')
    writer = csv.writer(fx)
    log.debug('generating ' + fx.name)

#    path(d / 'generated_template.c').write_text(TEMPLATE)
    for i, (snippet, comment) in enumerate(reader):
        snippet = snippet.replace('{LF}', '\n')
        fcode = d / 'generated_code4size_' + str(i) + '.c'
        path(fcode).write_text(snippet)

        msg = 'index: %s generating: %s ' % (i, fcode)
        log.debug(msg)
        logger(msg)

        size = simulator(snippet, mcu).size()
        empty_size = simulator('', mcu).size()

        writer.writerow([
                        comment,
                        '.. literalinclude:: ' + fcode.name,
                        size.program_bytes - empty_size.program_bytes,
                        size.data_bytes - empty_size.data_bytes,
                        ])
