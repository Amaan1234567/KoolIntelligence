#include "file_search.hpp"
#include <baloo/query.h>
/**
Reference for how this works and where it is taken from:
https://invent.kde.org/frameworks/baloo/-/blob/master/src/tools/baloosearch/main.cpp
https://invent.kde.org/frameworks/baloo/-/blob/master/src/lib/query.h

It will only return file paths for now. 
However the results are also based on the file contents as indexed by baloo/krunner. 
Note that this is KDE specific.
 */
std::vector<std::string> fileSearchKDE(const std::string& searchTerm){
    const uint numberOfResults = 10;
    Baloo::Query query;
    // query.addType(typeStr);
    query.setSearchString(QString().fromStdString(searchTerm));
    query.setLimit(numberOfResults);
    query.setSortingOption(Baloo::Query::SortAuto);
    // query.setIncludeFolder(canonicalPath);
    Baloo::ResultIterator iter = query.exec();
    std::vector<std::string> results;
    while (iter.next()) {
        const QString filePath = iter.filePath();
        results.push_back(filePath.toStdString());
    }
    return results;
}
