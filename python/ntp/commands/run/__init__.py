import ntp.commands

class Command(ntp.commands.Command):
    """
        Runs the n-tuple production.
        All run commands require a valid grid certificate as they either read
        data from the grid via XRootD or run on grid resources.
        
        Default is: 'run local' 
    """
    REQUIRE_GRID_CERT = True