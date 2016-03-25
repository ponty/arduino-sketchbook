from nose.tools import eq_, ok_
from path import path
from pyavrutils import support

root = path(__file__).parent.parent
examples = support.find_examples(root)


def test_examples_count():
    print examples
    ok_(len(examples))
