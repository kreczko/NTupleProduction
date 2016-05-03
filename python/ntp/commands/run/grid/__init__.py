"""
    run grid:    Uses CRAB3 to submit jobs to the WLCG grid
"""
from __future__ import print_function
from .. import Command as C


class Command(C):

    def __init__(self, path=__file__, doc=__doc__):
        super(Command, self).__init__(path, doc)

    def run(self, args, variables):
        self.__prepare(args, variables)
        self.__text = "NOT IMPLEMENTED - but would be running on the grid"

        return True
