Import("env")

#
# Upload actions
#

def before_build(source, target, env):
    print "****************before_build"
    # do some actions


def after_build(source, target, env):
    print "**********************after_build"
    # do some actions

def before_buildprg(source, target, env):
    print "****************before_buildprg"
    # do some actions


def after_buildprg(source, target, env):
    print "**********************after_buildprg"
    # do some actions

def before_buildsize(source, target, env):
    print "****************before_buildsize"
    # do some actions


def after_buildsize(source, target, env):
    print "**********************after_buildsize"
    # do some actions


print "***************************Current build targets", map(str, BUILD_TARGETS)

env.AddPreAction("build", before_build)
env.AddPostAction("build", after_build)

env.AddPreAction("buildprog", before_buildprg)
env.AddPostAction("buildprog", after_buildprg)

env.AddPreAction("size", before_buildsize)
env.AddPostAction("size", after_buildsize)
