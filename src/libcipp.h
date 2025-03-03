#ifndef LIBCIPP_H
#define LIBCIPP_H

// Include standard libraries
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <map>
#include <algorithm>

#include <zlib.h>
#include <openssl/sha.h>
#include "../dependencies/args.hxx"


#include "cippRepo.h"
#include "cippObject.h"

#include "cippCmd.h"

/*Arguments for the command line tool*/
ArgumentParser parser("This is a git clone program.", "This is after the options");

Group commands(parser, "commands");
Command init(commands, "init", "initializes the repo");
Command add(commands, "add", "adds file to be staged");
Command commit(commands, "commit", "Creates a new commit");

/*cat-file command with it's potitional arguments*/
Command cat_file(commands, "cat-file", "Provide content of repository objects");
Group cat_file_args(cat_file, "flags");
Positional<string> cat_file_type(cat_file_args, "type", "The type of object to display");
Positional<string> cat_file_object(cat_file_args, "object", "The object to display");

/*hash-file command with it's positional arguments*/
Command hash_object(commands, "hash-object", "Compute object ID and optionally creates a blob from a file");
Group hash_object_args(hash_object, "flags");
Group hash_object_flags(hash_object, "flags");
/*Hash command flags*/
Flag hash_object_write(hash_object_flags, "w", "Write the object into the database", {'w'});
ValueFlag<string> hash_object_path(hash_object_flags, "TYPE", "Specify the object type", {'t'});
Positional<string> hash_object_file(hash_object_args, "file", "The file to hash");


Group arguments(parser, "arguments", Group::Validators::DontCare, Options::Global);
HelpFlag help(parser, "help", "Display this help menu", {"h","help"});



CompletionFlag completion(parser, {"complete"});



#endif // LIBCIPP_H