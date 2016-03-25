from paver.easy import *
import logging
sys.path.insert(0, path('.').abspath())
import sim
from pyavrutils import support

# logging.basicConfig(level=logging.DEBUG)

import paver.doctools
import paver.virtual
import paver.misctasks
from paved import *
from paved.dist import *
from paved.util import *
from paved.docs import *
from paved.pycheck import *

options(
    sphinx=Bunch(
        docroot='docs',
        builddir="_build",
    ),
#    pdf=Bunch(
#        builddir='_build',
#        builder='latex',
#    ),
)


options.paved.clean.rmdirs += ['.tox',
                               'dist',
                               'build',
                               ]
options.paved.clean.patterns += ['*.pickle',
                                 '*.doctree',
                                 '*.gz',
                                 'nosetests.xml',
                                 'sloccount.sc',
                                 '*.pdf', '*.tex',
                                 '*.png',

                                 'generated_*',  # generated files

                                 '*.axf',
                                 '*.elf',
                                 '*.o',
                                 '*.a',
                                 '*.eep',
                                 '*.hex',
                                 '*.lss',
                                 '*.map',
                                 '*.lst',
                                 '*.sym',
                                 '*.vcd',
                                 'sgm7_hwconf.h',
                                 '*.bak',  # cheetah
                                 '*.zip',
                                 'distribute_setup.py',
                                 ]

options.paved.dist.manifest.include.remove('distribute_setup.py')
options.paved.dist.manifest.include.remove('paver-minilib.zip')

docroot = path(options.sphinx.docroot)
root = path(__file__).parent.parent.abspath()
examples = support.find_examples(root)


@task
@needs(
    'cog',
    'sloccount',
    'libsize',
    #       'snippet',
    'build_test',
    'boards',
    'doxy',
    'html',
    'pdf',
    'nose')
def alltest():
    'all tasks to check'
    pass


@task
def doxy():
    path('docs/_build/html/doxy').makedirs_p()
    sh('doxygen doxy.ini')


#@task
# def snippet():
#    '''generate screenshots from code snippets'''
#    f = docroot / 'code_examples.csv'
#    sim.snippet_doc(f, docroot, logger=info)

@task
def libsize():
    f = docroot / 'code4size.csv'
    sim.libsize(f, docroot, logger=info)


@task
def build_test():
    csv = docroot / 'generated_build_test.csv'
    support.build2csv(examples, csv, logdir=docroot / '_build' /
                      'html', extra_lib=root, logger=info)


@task
def boards():
    csv = docroot / 'generated_boards.csv'
    support.boards2csv(csv, logger=info)
