#include <iostream>
#include <stdio.h>

#include "gcc-plugin.h"
#include "plugin-version.h"

#include "tree-pass.h"
#include "context.h"
#include "tree.h"

/* must export this symbol */
int plugin_is_GPL_compatible;


struct plugin_gcc_version statetool_ver = 
{
    .basever = "13.2.0"
};

namespace
{
    const pass_data statetool_pass_data = 
    {
        GIMPLE_PASS, /* pass type */
        "statetool", /* name */
        OPTGROUP_NONE, /* optinfo_flags */
        TV_NONE, /* tv_id */
        PROP_gimple_any, /* properties required */
        0, /* properties provided */
        0, /* properties destroyed */
        0, /* todo_flags_start */
        0 /* todo_flags_finish */
    };

    struct state_pass : gimple_opt_pass
    {
        state_pass(gcc::context *ctx) : gimple_opt_pass(statetool_pass_data, ctx) {}

        virtual unsigned int execute(function *fun) override
        {
            basic_block bb;

            FOR_EACH_BB_FN(bb, fun)
            {
                std::cerr << "Hello! \n";
            }

            return 0;
        }

        virtual state_pass *clone() override { return this; }
    };
}

/* plugin entry point */
int plugin_init(struct plugin_name_args *plugin_info, struct plugin_gcc_version *version)
{
    /* version check */
    if(!plugin_default_version_check(version, &gcc_version))
        return 1;


    register_callback(plugin_info->base_name, PLUGIN_INFO, NULL, plugin_info);

    /* register pass after cfg */
    struct register_pass_info pass_info;

    pass_info.pass = new state_pass(g);
    pass_info.reference_pass_name = "cfg";
    pass_info.ref_pass_instance_number = 1;
    pass_info.pos_op = PASS_POS_INSERT_AFTER;

    register_callback(plugin_info->base_name, PLUGIN_PASS_MANAGER_SETUP, NULL, &pass_info);

    /* plugin finish */
    // register_callback(plugin_info->base_name, PLUGIN_FINISH, NULL, NULL);

    return 0;
}