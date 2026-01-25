#include <iostream>
#include <fstream>
#include <filesystem>

#include "../graph.hpp"
#include "cg/viz/dot.hpp"

namespace fs = std::filesystem;

namespace cg::viz {

    template<Numeric T>
    void visualize(const Graph<T>& G) {
        fs::path project_root = fs::path(__FILE__).parent_path().parent_path();
        fs::path out_dir = project_root / "viz_output";
        fs::create_directories(out_dir);

        fs::path dot_path = out_dir / "graph.dot";
        std::ofstream dot_file(dot_path);

        viz::export_to_dot(G, dot_file);
        dot_file.close();

        std::cout << "\nrendering png...\n";
        std::string cmd = "dot -Tpng " + dot_path.string() + " -o " + (out_dir / "graph.png").string();
        int ret = std::system(cmd.c_str());

        if (ret == 0) {
            std::cout << "success <3 visualization rendered to: " << out_dir << "/graph.png\n";
            // std::system("mimeopen -d graph.png");
        }
    }
}