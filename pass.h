// #pragma once

// #include <algorithm>
// #include <fstream>
// #include <functional>
// #include <numeric>
// #include <string>

// #include "scenebasic_uniform.h"

// struct Pass {
//   std::string src;
//   std::function<void(SceneBasic_Uniform &)> *init;
//   std::function<void(SceneBasic_Uniform &)> *render;
// };

// struct Passes {
//   std::vector<Pass> passes;

//   std::string buildShader() {
//     std::stringstream buf;
//     buf << "uniform uint pass;\n\n";

//     auto i = 0;
//     for (auto &pass : passes) {
//       // std::string filename = "shader/" + pass.name + ".frag";
//       // std::ifstream file(filename);
//       // buf << file.rdbuf() << "\n\n";
//       // file.close();
//       buf << "void pass" << std::to_string(i) << "() {\n";
//       buf << pass.src;
//       buf << "}\n\n";
//       i++;
//     }

//     buf << "\n\nvoid main() {\n";

//     auto i = 0;
//     for (auto &pass : passes) {
//       buf << "   if (pass == " << std::to_string(i) << ") " << "color = pass"
//           << std::to_string(i) << "();\n";
//       i++;
//     }

//     buf << "\n}";
//     return buf.str();
//   }

//   void init(SceneBasic_Uniform &sb) {
//     for (auto &pass : passes) {
//       (*pass.init)(sb);
//     }
//   }

//   void render(SceneBasic_Uniform &sb) {
//     auto i = 0;
//     for (auto &pass : passes) {
//       sb.prog.setUniform("pass", i);
//       (*pass.render)(sb);
//       // glFlush();
//       i++;
//     }
//   }
// };
