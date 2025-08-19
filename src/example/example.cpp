/*
@novbytes - 2025
*/

#include <iostream>
#include <string>
#include <vector>
#include <thread>

#include "../single/sputil.hpp"

using namespace sputil;

int main()
{
    std::cout << "=== Example ===\n\n";

    // ---------------------------------------------
    // 1. TIME
    // ---------------------------------------------
    std::cout << "[TIME] Sleeping for 1.5 seconds...\n";
    time::sleep(1.5); 
    std::cout << "[TIME] Done!\n\n";

    {
        time::Timer t;
        time::sleep(0.3);
        std::cout << "[TIME] Elapsed: " << t.elapsed_ms() << " ms\n\n";
    }

    // ---------------------------------------------
    // 2. ARRAY
    // ---------------------------------------------
    std::vector<std::string> names = {"Andi", "Budi", "Cici", "Dedi", "Eka"};
    std::cout << "[ARRAY] Before shuffle: ";
    for (auto &n : names) std::cout << n << " ";
    std::cout << "\n";

    array::shuffle(names);

    std::cout << "[ARRAY] After shuffle:  ";
    for (auto &n : names) std::cout << n << " ";
    std::cout << "\n\n";

    // ---------------------------------------------
    // 3. STRING
    // ---------------------------------------------
    std::string text = "  Hello World  ";
    std::cout << "[STRING] Original: '" << text << "'\n";
    std::cout << "[STRING] Trimmed : '" << string::trim(text) << "'\n";
    std::cout << "[STRING] Upper   : '" << string::to_upper(text) << "'\n";
    std::cout << "[STRING] Lower   : '" << string::to_lower(text) << "'\n\n";

    // ---------------------------------------------
    // 4. THREADING (ThreadPool)
    // ---------------------------------------------
    std::cout << "[THREAD] Launching ThreadPool with 4 workers\n";
    threading::ThreadPool pool(4);

    for (int i = 0; i < 8; i++) {
        pool.enqueue([i]() {
            std::cout << "Task " << i << " running on thread " 
                      << std::this_thread::get_id() << "\n";
            sputil::time::sleep(0.2);
        });
    }

    std::cout << "\n";

    // ---------------------------------------------
    // 5. DEBUG - Fix this soon!
    // ---------------------------------------------
    // {
    //     SPTR_SCOPE_TIMER();
    //     time::sleep(0.5);
    // }
    // std::cout << "[DEBUG] ScopeTimer was finished successfully.\n\n";
}
