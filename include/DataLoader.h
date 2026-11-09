#ifndef DATALOADER_H
#define DATALOADER_H
#include <string>
#include "CampusGraph.h"
#include "DepartmentTree.h"
#include "FacilityStore.h"


void loadAllData(const std::string& dir, CampusGraph& graph,
                 FacilityStore& facilities, DepartmentTree& tree);
#endif
