"""
    run:    Runs the n-tuple production.
            All run commands require a valid grid certificate as they
            either read data from the grid via XRootD or run on grid
            resources.
            Usage:
                run [grid|condor|local] [sample=X]
            default:
                run local sample=Test
"""

import ntp.commands


class Command(ntp.commands.Command):
    REQUIRE_GRID_CERT = True

    def __init__(self, path=__file__, doc=__doc__):
        super(Command, self).__init__(path, doc)
