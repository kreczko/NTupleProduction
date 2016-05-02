"""
    run grid:    Uses CRAB3 to submit jobs to the WLCG grid
"""
from __future__ import print_function
from .. import Command as C


class Command(C):

    def __init__(self, path=__file__, doc=__doc__):
        super(Command, self).__init__(path, doc)

    def run(self):
        print("Running on the grid")
