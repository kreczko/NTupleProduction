"""
    run local:  Runs the n-tuple production on the current machine.
                All run commands require a valid grid certificate as they
                either read data from the grid via XRootD or run on grid
                resources.
        Usage:
            run local [sample=<X>]
        Parameters:
            sample:   Which sample to run over.
                      Default: test
"""
from __future__ import print_function
import ntp.commands


class Command(ntp.commands.Command):

    def __init__(self, path=__file__, doc=__doc__):
        super(Command, self).__init__(path, doc)

    def run(self):
        print("Running locally")
