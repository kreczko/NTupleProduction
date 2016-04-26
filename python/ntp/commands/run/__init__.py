"""
    run:    Runs the n-tuple production.
            All run commands require a valid grid certificate as they
            either read data from the grid via XRootD or run on grid
            resources.
        Usage:
            run [<where>] [sample=<X>]
        Parameters:
            where:    Where to run NTP. Can be grid|condor|local.
                      For location specific parameters, please run
                        help run <where>
                      Default: local
            sample:   Which sample to run over.
                      Default: test
"""

import ntp.commands


class Command(ntp.commands.Command):
    REQUIRE_GRID_CERT = True

    def __init__(self, path=__file__, doc=__doc__):
        super(Command, self).__init__(path, doc)
