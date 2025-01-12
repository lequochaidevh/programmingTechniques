#pragma once

#include <iostream>
#include <vector>

namespace nsStrategy {
    class SortingStrategy {
    public:
        virtual void sort(std::vector<int>& arr) = 0;
    };
    class BubbleSort : public SortingStrategy {
    public:
        void sort(std::vector<int>& arr) override
        {
            // Implement Bubble Sort algorithm
            std::cout << "Implement Bubble Sort algorithm \n";
        }
    };

    class QuickSort : public SortingStrategy {
    public:
        void sort(std::vector<int>& arr) override
        {
            // Implement Quick Sort algorithm
            std::cout << "Implement Quick Sort algorithm \n";
        }
    };

    // Add more sorting algorithms as needed

    class SortContext {
    private:
        SortingStrategy* strategy;

    public:
        void setStrategy(SortingStrategy* strategy)
        {
            this->strategy = strategy;
        }

        void executeStrategy(std::vector<int>& arr)
        {
            strategy->sort(arr);
        }
    };
    void programTest()
    {
        std::vector<int> data = { 5, 2, 7, 1, 9 };

        SortContext context;
        BubbleSort bubbleSort;
        QuickSort quickSort;

        context.setStrategy(&bubbleSort);
        context.executeStrategy(data); // Executes Bubble Sort

        context.setStrategy(&quickSort);
        context.executeStrategy(data); // Executes Quick Sort

    }
}