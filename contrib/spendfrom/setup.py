from distutils.core import setup
setup(name='mgcspendfrom',
      version='1.0',
      description='Command-line utility for floripacoin "coin control"',
      author='Gavin Andresen',
      author_email='gavin@floripacoinfoundation.org',
      requires=['jsonrpc'],
      scripts=['spendfrom.py'],
      )
