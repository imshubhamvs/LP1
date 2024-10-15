#include <iostream>
#include <vector>
using namespace std;

// Function to get memory block sizes
vector<int> get_block_input() {
    int n;
    cout << "Enter number of memory block(s): ";
    cin >> n;
    
    vector<int> block_size(n);
    cout << "Enter the size of memory blocks: ";
    for (int i = 0; i < n; i++) {
        cin >> block_size[i];
    }
    
    return block_size;
}

// Function to get process sizes
vector<int> get_process_input() {
    int n;
    cout << "Enter the number of processes: ";
    cin >> n;
    
    vector<int> proc_size(n);
    cout << "Enter the size of processes: ";
    for (int i = 0; i < n; i++) {
        cin >> proc_size[i];
    }
    
    return proc_size;
}

// First Fit algorithm
void first_fit(vector<int>& block_size, const vector<int>& proc_size) {
    vector<int> allotted_blocks(proc_size.size(), -1);
    vector<int> fragmentation(proc_size.size(), 0);

    for (int i = 0; i < proc_size.size(); i++) {
        for (int j = 0; j < block_size.size(); j++) {
            if (block_size[j] >= proc_size[i]) {
                allotted_blocks[i] = j;
                fragmentation[i] = block_size[j] - proc_size[i];
                block_size[j] -= proc_size[i];
                break;
            }
        }
    }

    // Output result
    cout << "\nFirst Fit Allocation: \n";
    for (int i = 0; i < proc_size.size(); i++) {
        if (allotted_blocks[i] != -1)
            cout << "Process " << i+1 << " is allocated to Block " << allotted_blocks[i]+1 << " with fragmentation: " << fragmentation[i] << "\n";
        else
            cout << "Process " << i+1 << " is not allocated.\n";
    }
}

// Best Fit algorithm
void best_fit(vector<int>& block_size, const vector<int>& proc_size) {
    vector<int> allotted_blocks(proc_size.size(), -1);
    vector<int> fragmentation(proc_size.size(), 0);

    for (int i = 0; i < proc_size.size(); i++) {
        int best_idx = -1;
        for (int j = 0; j < block_size.size(); j++) {
            if (block_size[j] >= proc_size[i]) {
                if (best_idx == -1 || block_size[best_idx] > block_size[j]) {
                    best_idx = j;
                }
            }
        }

        if (best_idx != -1) {
            allotted_blocks[i] = best_idx;
            fragmentation[i] = block_size[best_idx] - proc_size[i];
            block_size[best_idx] -= proc_size[i];
        }
    }

    // Output result
    cout << "\nBest Fit Allocation: \n";
    for (int i = 0; i < proc_size.size(); i++) {
        if (allotted_blocks[i] != -1)
            cout << "Process " << i+1 << " is allocated to Block " << allotted_blocks[i]+1 << " with fragmentation: " << fragmentation[i] << "\n";
        else
            cout << "Process " << i+1 << " is not allocated.\n";
    }
}

// Worst Fit algorithm
void worst_fit(vector<int>& block_size, const vector<int>& proc_size) {
    vector<int> allotted_blocks(proc_size.size(), -1);
    vector<int> fragmentation(proc_size.size(), 0);

    for (int i = 0; i < proc_size.size(); i++) {
        int worst_idx = -1;
        for (int j = 0; j < block_size.size(); j++) {
            if (block_size[j] >= proc_size[i]) {
                if (worst_idx == -1 || block_size[worst_idx] < block_size[j]) {
                    worst_idx = j;
                }
            }
        }

        if (worst_idx != -1) {
            allotted_blocks[i] = worst_idx;
            fragmentation[i] = block_size[worst_idx] - proc_size[i];
            block_size[worst_idx] -= proc_size[i];
        }
    }

    // Output result
    cout << "\nWorst Fit Allocation: \n";
    for (int i = 0; i < proc_size.size(); i++) {
        if (allotted_blocks[i] != -1)
            cout << "Process " << i+1 << " is allocated to Block " << allotted_blocks[i]+1 << " with fragmentation: " << fragmentation[i] << "\n";
        else
            cout << "Process " << i+1 << " is not allocated.\n";
    }
}

// Next Fit algorithm
void next_fit(vector<int>& block_size, const vector<int>& proc_size) {
    vector<int> allocation(proc_size.size(), -1);
    int j = 0;  // Start from the first block
    
    for (int i = 0; i < proc_size.size(); i++) {
        int starting_block = j;
        bool allocated = false;
        
        while (true) {
            if (block_size[j] >= proc_size[i]) {
                allocation[i] = j;
                block_size[j] -= proc_size[i];
                allocated = true;
                break;
            }
            j = (j + 1) % block_size.size();
            if (j == starting_block) break;  // We checked all blocks and couldn't find a fit
        }
        
        if (!allocated) {
            allocation[i] = -1;  // Process not allocated
        }
    }

    // Output result
    cout << "\nNext Fit Allocation: \n";
    for (int i = 0; i < proc_size.size(); i++) {
        if (allocation[i] != -1)
            cout << "Process " << i+1 << " is allocated to Block " << allocation[i]+1 << "\n";
        else
            cout << "Process " << i+1 << " is not allocated.\n";
    }
}

int main() {
    vector<int> block_size = get_block_input();
    vector<int> proc_size = get_process_input();
    
    vector<int> block_size_copy = block_size;  // Backup original block sizes

    first_fit(block_size_copy, proc_size);
    
    block_size_copy = block_size;  // Reset block sizes for the next algorithm
    best_fit(block_size_copy, proc_size);
    
    block_size_copy = block_size;
    worst_fit(block_size_copy, proc_size);
    
    block_size_copy = block_size;
    next_fit(block_size_copy, proc_size);
    
    return 0;
}
