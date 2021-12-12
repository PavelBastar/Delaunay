#include "IDManager.h"
#include <iostream>


IDManager::IDManager()
{
    available_IDs.push_back(0);
}

uint32_t IDManager::getID()
{
    if(available_IDs.size() > 1)
    {

        uint32_t id = available_IDs.back();
        available_IDs.pop_back();
        return id;
    }
    else
    {
        return (available_IDs.front()++);
    }

}


void IDManager::freeID(uint32_t id)
{
    available_IDs.push_back(id);
}

bool IDManager::isIDused(uint32_t id) const
{
    int num_IDs = static_cast<int>(available_IDs.size()-1);
    for(int i = num_IDs; i > 0; i--)
    {
        if (id == available_IDs[i])
        {
            return false;
        }
        if (id < available_IDs[0])
        {
            return true;
        }
        else
        {
            return false;
        }

    }

}

uint32_t IDManager::numberUsedIDs() const
{
    return available_IDs.front() - (static_cast<uint32_t>(available_IDs.size())) + 1;

}


uint32_t IDManager::numFreeIDs() const
{
    return static_cast<uint32_t>(available_IDs.size()) - 1;
}

uint32_t IDManager::peekNextID() const
{
    return available_IDs[0];
}
