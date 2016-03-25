from nose.tools import eq_, ok_
from path import path
from pyavrutils import support, arduino
from pyavrutils.arduino import Arduino, ArduinoCompileError
import logging

root = path(__file__).parent.parent
examples = support.find_examples(root)

fails = [
    ('PWM.pde', 'atmega8'),
]


# def check_build(ex, hwpack, board):
#    cc = Arduino(hwpack=hwpack, board=board)
#    cc.extra_lib = root
#    print cc.hwpack, cc.board, ex
#    print (str(path(ex).name), cc.mcu_compiler())
#    if (str(path(ex).name), cc.mcu_compiler()) in fails:
#        class Dummy(TestCase):
#            def runTest(self):
#                pass
#        Dummy().assertRaises(ArduinoCompileError, cc.build, cc, ex)
#    else:
#        cc.build(ex)
#        assert cc.size().ok
def check_build(ex, hwpack, board):
    cc = Arduino(hwpack=hwpack, board=board)
#    cc.extra_lib = root
    print cc.hwpack, cc.board, ex
    cc.build(ex)
    assert cc.size().ok


def generate(func, params, labels=None):
    if not labels:
        labels = params
    if not hasattr(func, '_index'):
        func._index = 0
    func._index += 1
    cmd = '''def test_{func._index}_{labels}(): {func.__name__}({params})'''.format(func=func,
                                                                                    params=','.join(
                                                                                    ['"%s"' % x for x in params]),
                                                                                    labels='_'.join(labels))
    logging.debug('cmd:' + cmd)
    return cmd

# def test_build():
#    for ex in examples:
#        for cc in arduino.targets():
#            cc.extra_lib = root
#            if cc.hwpack=='arduino':
#                yield check_build, ex, cc
for ex in examples:
    for cc in arduino.targets():
        if cc.hwpack == 'arduino':
            if (str(path(ex).name), cc.mcu_compiler()) not in fails:
                exec generate(check_build,
                              [ex, cc.hwpack, cc.board],
                              [ex.namebase, cc.hwpack, cc.board])
