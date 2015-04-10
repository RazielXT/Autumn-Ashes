#pragma once

template<typename T>
class ListLoop
{
public:
    ListLoop(T data, bool is_first = true, bool is_last = true) : first(is_first), last(is_last), value(data)
    {
        previous = next = this;
    }

    ListLoop() : first(true), last(true)
    {
        previous = next = this;
    }

    ~ListLoop()
    {
        if (previous)
            clearAll();
    }

    ListLoop<T>* next;
    ListLoop<T>* previous;
    T value;

    ListLoop<T>* addToEnd(T data)
    {
        auto it = this;
        while (!it->last) it = it->next;

        it->last = false;

        auto added = new ListLoop<T>(data, false, true);
        added->previous = it;
        added->next = it->next;

        it->next->previous = added;
        it->next = added;

        return added;
    }

protected:

    void clearAll()
    {
        auto it = this;
        while (!it->first) it = it->previous;

        while (!it->last)
        {
            auto l = it;
            it = it->next;

            l->previous = nullptr;

            if (l != this)
                delete l;
        }
    }

    bool last, first;
};