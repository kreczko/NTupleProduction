import getpass

HDFS_STORE_BASE = "/hdfs/TopQuarkGroup/{user}".format(
    user=getpass.getuser()
)
