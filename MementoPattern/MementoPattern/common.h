#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>

class Picture;
class PictureEditor;
class Snapshot;

using std::vector;
using std::string;
using std::to_string;
using std::shared_ptr;
using std::make_shared;
using std::cout;
using std::endl;
using std::cin;
using Picturep = shared_ptr<Picture>;
using PictureEditorp = shared_ptr<PictureEditor>;
using Snapshotp = shared_ptr<Snapshot>;

#endif