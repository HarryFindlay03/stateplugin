/***
 * AUTHOR: Harry Findlay
 * LICENSE: Shipped with package - GNU GPL v3.0
 * FILE START: 02/05/2024
 * FILE LAST UPDATED: 08/05/2024
 * 
 * REQUIREMENTS: GCC v13.2.0
 * REFERENCES: GCC Interals Documentation - https://gcc.gnu.org/onlinedocs/gccint/index.html
 * 
 * DESCRIPTION: Implementation file for state extractor gcc plugin for deep-rl optimizer
*/


#include "statetool.h"


std::vector<std::string> feat_vec = 
{
    "cond",
    "goto",
    "label",
    "switch",
    "assign",
    "asm",
    "call",
    "transaction",
    "return",
    "bind",
    "phi",
    "nop"
};


/* state_pass IMPLEMENTATION */


state_pass::state_pass(gcc::context *ctx, const std::string& filename)
: filename(filename), gimple_opt_pass(statetool_pass_data, ctx)
{
    // init features
    for(auto v : feat_vec)
        features.emplace(v, 0);

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
            gimple_analyser(features, gsi_stmt(gsi));
        }
    }

    file_writer(filename, features);

    return 0;
}


/* GIMPLE ANALYSIS FUNCTIONS */


void gimple_analyser(std::map<std::string, size_t>& feat, const gimple* g_stmt)
{
    switch(g_stmt->code)
    {
        case GIMPLE_COND:
            feat["cond"] += 1;
            break;
        case GIMPLE_GOTO:
            feat["goto"] += 1;
            break;
        case GIMPLE_LABEL:
            feat["label"] += 1;
            break;
        case GIMPLE_SWITCH:
            feat["switch"] += 1;
            break;
        case GIMPLE_ASSIGN:
            feat["assign"] += 1;
            break;
        case GIMPLE_ASM:
            feat["asm"] += 1;
            break;
        case GIMPLE_CALL:
            feat["call"] += 1;
            break;
        case GIMPLE_TRANSACTION:
            feat["transaction"] += 1;
            break;
        case GIMPLE_RETURN:
            feat["return"] += 1;
            break;
        case GIMPLE_BIND:
            feat["bind"] += 1;
            break;
        case GIMPLE_PHI:
            feat["phi"] += 1;
            break;
        case GIMPLE_NOP:
            feat["nop"] += 1;
            break;
        default:
            break;
    }

    return;
}


void file_writer(const std::string& filename, const std::map<std::string, size_t>& m)
{
    std::ofstream file_output(filename.c_str());

    if(file_output.is_open())
    {
        // output values to file - ordered map so no need to check for key ordering!
        for(auto it = m.begin(); it != m.end(); ++it)
            file_output << it->second << '\n';
    }

    file_output.close();

    return;
}



/* PLUGIN ENTRY POINT */
int plugin_init(struct plugin_name_args *plugin_info, struct plugin_gcc_version *version)
{
    /* version check */
    if(!plugin_default_version_check(version, &gcc_version))
        return 1;
    
    /* register plugin info to plugin api */
    register_callback(plugin_info->base_name, PLUGIN_INFO, NULL, plugin_info);

    /* checking filename argument has been passed */
    if((plugin_info->argc == 0) || (plugin_info->argc > 1) || strcmp((plugin_info->argv[0]).key, "filename"))
    {
        std::cerr << "plugin filename argument has not been passed, please only pass with: -fplugin-arg-statetool-filename=filename.txt\n";
        return 1;
    }

    /* registering custom pass */
    struct register_pass_info pass_info;

    pass_info.pass = new state_pass(g, (std::string)plugin_info->argv[0].value);
    pass_info.reference_pass_name = "cfg"; // register pass after cfg, this needs to change ???
    pass_info.ref_pass_instance_number = 1;
    pass_info.pos_op = PASS_POS_INSERT_AFTER;

    register_callback(plugin_info->base_name, PLUGIN_PASS_MANAGER_SETUP, NULL, &pass_info);


    return 0;
}