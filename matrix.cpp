#include <iostream>
#include <map>
#include <tuple>
#include <cassert>
 
using Index = std::pair<int, int>;
 
template<typename T, T N>
class SparseMatrix;
 
template<typename T, T N = -1 >
class SubMatrix
{
public:
    SubMatrix(SparseMatrix<T, N> *_matrixPtr, int _row)
        : matrixPtr(_matrixPtr)
        , row(_row)
    {}
 
    SubMatrix<T, N> &operator[](int col)
    {
        index = std::make_pair(row, col);
        elem = matrixPtr->getValue(index);
        return *this;
    }
 
    void operator=(T value)
    {
        if (value != matrixPtr->getDefaultElem())
        {
            elem = value;
            matrixPtr->setValue(index, elem);
        }
        else
        {
            if (elem != matrixPtr->getDefaultElem())
                matrixPtr->removeByIndex(index);
        }
    }
 
    operator T()
    {
        return elem;
    }
 
private:
    SparseMatrix<T, N> *matrixPtr;
    int row;
    T elem;
    Index index;
};
 
template<typename T, T N = -1 >
class SparseMatrix
{
public:
    using MatrixIterator = typename std::map<Index, T>::iterator;
 
    SparseMatrix() : defaultElem(N) {}
   
    T getDefaultElem() const
    {
        return defaultElem;
    }
 
    void setValue(Index index, T val)
    {
        matrix[index] = val;
    }
 
    void removeByIndex(const Index &index)
    {
        matrix.erase(matrix.find(index));
    }
 
    T& getValue(const Index &index)
    {
        auto it = matrix.find(index);
        return it == matrix.end() ? defaultElem : it->second;
    }
 
    SubMatrix<T, N> operator[](int row)
    {
        SubMatrix<T, N> subMatrix(this, row);
        return subMatrix;
    }
 
    size_t size() const
    {
        return matrix.size();
    }
 
    class Iterator
    {
    public:
        Iterator(MatrixIterator _it)
            : it(_it)
        {}
 
        void operator++()
        {
            ++it;
        }
       
        std::tuple<int, int, T> operator*()
        {
            return std::make_tuple(it->first.first, it->first.second, it->second);
        }
 
        bool operator!=(const Iterator &other)
        {
            return it != other.it;
        }
 
        bool operator==(const Iterator &other)
        {
            return it == other.it;
        }
    private:
        MatrixIterator it;
    };
 
    Iterator begin()
    {
        return Iterator(matrix.begin());
    }
 
    Iterator end()
    {
        return Iterator(matrix.end());
    }
 
private:
    T defaultElem;
    std::map<Index, T> matrix;
};
 
 
int main(int argc, char const *argv[]) 
{
 
    SparseMatrix<int, 0> matrix;
 
    assert(matrix.size() == 0);
    auto a = matrix[0][0];
    assert(a == 0);
    assert(matrix.size() == 0);
    matrix[100][100] = 314;
    assert(matrix[100][100] == 314);
    assert(matrix.size() == 1);
    matrix[100][100] = 0;
    assert(matrix[100][100] == 0);
    assert(matrix.size() == 0);
 
 
    for (auto i = 0; i < 10; i++)
        matrix[i][i] = i;
    for (auto i = 0, j = 9; i < 10; i++, j--)
        matrix[i][j] = j;
 
    std::cout << "size: " << matrix.size() << std::endl;
 
    for (auto c : matrix) {
        int x, y, v;
        std::tie(x, y, v) = c;
        std::cout << "[" << x << "]" << "[" << y << "]" << " value: " << v << std::endl;
    }
 
    for (auto i = 1; i < 9; i++) {
        for (auto j = 1; j < 9; j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
   
    return 0;
}