"""
    list samples:    lists all available samples
"""
from .. import Command as C

class Command(C):
    def __init__(self, path = __file__, doc = __doc__):
        super(Command, self).__init__(path, doc)

    def run(self, params, args):
        print('Listing all known samples')
