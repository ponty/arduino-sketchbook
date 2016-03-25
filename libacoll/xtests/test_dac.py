from nose.tools import eq_, ok_
from path import path
import sim

root = path(__file__).parent.parent
# examples = support.find_examples(root)


TEMPLATE = '''
#include <coll.h>
OpAmpDac dac(0, 0);
void setup()
{
    Serial.begin(9600);
    dac.begin();
    bool ok = dac.write($value);
    Serial.print(ok);
}

void loop()
{
}
'''


def check_sim(mcu, value, ouput):
    prog = TEMPLATE.replace('$value', str(value))
    print prog
    eq_(sim.simulator(code=prog, mcu=mcu).get_serial(), ouput)


def check(mcu):
    # out is always zero
    check_sim(mcu, value=0, ouput='1')
    check_sim(mcu, value=10, ouput='0')

for mcu in sim.targets():
    exec '''
def test_{mcu}():
    check("{mcu}")
'''.format(mcu=mcu)
