from nose.tools import eq_, ok_
from path import path
import sim

root = path(__file__).parent.parent
# examples = support.find_examples(root)


def check_sim(snippet, mcu, value):
    result = int(sim.simulator(snippet, mcu).get_serial())
    print result, value
    ok_(result - value < 100)


def test_sim():
    # this values can change with new arduino version
    check_sim('Serial.print(freeMemory1());', 'atmega48', 315)
    check_sim('Serial.print(freeMemory1());', 'atmega88', 827)
    check_sim('Serial.print(freeMemory2());', 'atmega168', 820)
    check_sim('Serial.print(freeMemory2());', 'atmega328p', 1844)
    check_sim('Serial.print(freeMemory3());', 'atmega48', 272)
    check_sim('Serial.print(freeMemory3());', 'atmega168', 784)
