"""
    list:   Used to list things. By default it will list the things to list
        Usage:
                list <thing to list>
"""
from .. import Command as C

class Command(C):
    def __init__(self, path = __file__, doc = __doc__):
        super(Command, self).__init__(path, doc)
