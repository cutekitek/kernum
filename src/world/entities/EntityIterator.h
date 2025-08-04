#ifndef ENTITYITERATOR_H
#define ENTITYITERATOR_H


template <typename T>
class TypeFilterIterator {
public:
    using BaseIter = std::vector<std::unique_ptr<Entity>>::const_iterator;

    TypeFilterIterator(BaseIter current, BaseIter end)
        : current(current), end(end) {
        advanceToNextValid();
    }

    T* operator*() const {
        return static_cast<T*>(current->get());
    }

    TypeFilterIterator& operator++() {
        ++current;
        advanceToNextValid();
        return *this;
    }

    bool operator!=(const TypeFilterIterator& other) const {
        return current != other.current;
    }

private:
    BaseIter current;
    BaseIter end;

    void advanceToNextValid() {
        while (current != end && (*current)->GetType() != EntityTraits<T>::type) {
            ++current;
        }
    }
};

template <typename T>
class EntityRange {
public:
    using BaseIter = std::vector<std::unique_ptr<Entity>>::const_iterator;

    EntityRange(BaseIter begin, BaseIter end)
        : beginIter(begin), endIter(end) {}

    TypeFilterIterator<T> begin() const { return {beginIter, endIter}; }
    TypeFilterIterator<T> end() const { return {endIter, endIter}; }

private:
    BaseIter beginIter;
    BaseIter endIter;
};
#endif //ENTITYITERATOR_H
