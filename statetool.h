/***
 * AUTHOR: Harry Findlay
 * LICENSE: Shipped with package - GNU GPL v3.0
 * FILE START: 06/05/2024
 * FILE LAST UPDATED: 08/05/2024
 * 
 * REQUIREMENTS: GCC v13.2.0
 * REFERENCES: GCC Interals Documentation - https://gcc.gnu.org/onlinedocs/gccint/index.html
 * 
 * DESCRIPTION: Header file for state extractor gcc plugin for deep-rl optimizer
*/


#include <iostream>
#include <stdio.h>
#include <vector>
#include <map>
#include <string>
#include <fstream>

#include "gcc-plugin.h"
#include "plugin-version.h"

#include "tree.h" //required for gimple definitions
#include "tree-pass.h"
#include "context.h"
#include "pass_manager.h"

#include "gimple.h"
#include "gimple-iterator.h"


#define NUM_FEATURES (int)12


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
    std::map<std::string, size_t> features;// warn overflow possibility
    std::string filename;

    state_pass(gcc::context *ctx, const std::string& filename);

    virtual unsigned int execute(function *fun) override;

    virtual state_pass *clone() override { return this; }
};


/* gimple state analysis */
void gimple_analyser(std::map<std::string, size_t>& feat, const gimple* g_stmt);

void file_writer(const std::string& filename, const std::map<std::string, size_t>& m);


/* help for debug */
inline void print_map(const std::map<std::string, size_t>& m)
{
    for(const auto& [k, v] : m)
        std::cout << '[' << k << "] = " << v << '\n';
    
    return;
}