#include "statetool.h"



/* state_pass IMPLEMENTATION */

state_pass::state_pass(gcc::context *ctx)
: gimple_opt_pass(statetool_pass_data, ctx)
{
    // init features
    features["num_ops"] = 0;
    features["mem_refs"] = 0;
    // ...

    if(features.size() != NUM_FEATURES)
    {
        std::cerr << "number of features does not match number of features in features map, please update.\n";
        std::exit(-1);
    }

    return;
}


unsigned int state_pass::execute(function *fun)
{
    basic_block bb;

    FOR_EACH_BB_FN(bb, fun)
    {
        /* sequence iterator for basic block */
        gimple_stmt_iterator gsi;

        for(gsi = gsi_start_bb(bb); !gsi_end_p(gsi); gsi_next(&gsi))
        {
            gimple* g = gsi_stmt(gsi);

            features["num_ops"] += g->num_ops;
            features["mem_refs"] += (1 ? gimple_references_memory_p(g) : 0);
        }
    }

    // todo write features to file
    print_map(features);

    return 0;
}


/* PLUGIN ENTRY POINT */
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