#ifndef IDMANAGER_H_INCLUDED
#define IDMANAGER_H_INCLUDED

#include <vector>
#include <stdint.h>


typedef std::vector<uint32_t> IDList;

class IDManager
{
private:
    IDList available_IDs;
public:
    IDManager();
    ~IDManager() { }

    uint32_t getID();

    void freeID(uint32_t);

    bool isIDused(uint32_t id) const;

    uint32_t numberUsedIDs() const;

    uint32_t numFreeIDs() const;

    uint32_t peekNextID() const;
};


#endif // IDMANAGER_H_INCLUDED
