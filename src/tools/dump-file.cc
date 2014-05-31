#include <gflags/gflags.h>
#include <stdlib.h>
#include <string.h>

#include <string>

#include "dump_load.h"
#include "codesearch.h"
#include "content.h"
#include "debug.h"
#include "indexer.h"
#include "re_width.h"


void dump_file(code_searcher *cs, indexed_file *f) {
    for (auto it = f->content->begin(cs->alloc());
         it != f->content->end(cs->alloc()); ++it) {
        printf("%.*s\n", it->size(), it->data());
    }
}

int main(int argc, char **argv) {
    google::SetUsageMessage("Usage: " + string(argv[0]) + " <options> INDEX PATH");
    google::ParseCommandLineFlags(&argc, &argv, true);

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <options> INDEX PATH\n", argv[0]);
        return 1;
    }

    string index = argv[1];
    string path = argv[2];

    code_searcher cs;
    cs.load_index(index);

    for (auto it = cs.begin_files(); it != cs.end_files(); ++it) {
        for (auto pit = (*it)->paths.begin(); pit != (*it)->paths.end(); ++pit) {
            if (pit->path == path) {
                dump_file(&cs, *it);
                return 0;
            }
        }
    }

    fprintf(stderr, "No files matching path: %s\n", path.c_str());

    return 0;
}
