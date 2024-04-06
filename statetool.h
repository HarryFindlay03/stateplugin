#include <iostream>
#include <stdio.h>

#include "gcc-plugin.h"
#include "plugin-version.h"

#include "tree.h" //required for gimple definitions
#include "tree-pass.h"
#include "context.h"

#include "gimple.h"
#include "gimple-iterator.h"


/* must export this symbol */
int plugin_is_GPL_compatible;


struct plugin_gcc_version statetool_ver = 
{
    .basever = "13.2.0"
};


const pass_data statetool_pass_data =
{
    GIMPLE_PASS,     /* pass type */
    "statetool",     /* name */
    OPTGROUP_NONE,   /* optinfo_flags */
    TV_NONE,         /* tv_id */
    PROP_gimple_any, /* properties required */
    0,               /* properties provided */
    0,               /* properties destroyed */
    0,               /* todo_flags_start */
    0                /* todo_flags_finish */
};


/* state_pass DEF */
struct state_pass : gimple_opt_pass
{
    state_pass(gcc::context *ctx) : gimple_opt_pass(statetool_pass_data, ctx) {}

    virtual unsigned int execute(function *fun) override;

    virtual state_pass *clone() override { return this; }
};