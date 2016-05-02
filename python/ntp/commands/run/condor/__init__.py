"""
    run condor:    Submits requested job to the HTCondor cluster
"""
from __future__ import print_function
from .. import Command as C


class Command(C):

    def __init__(self, path=__file__, doc=__doc__):
        super(Command, self).__init__(path, doc)

    def run(self):
        print("Bye, bye!")
