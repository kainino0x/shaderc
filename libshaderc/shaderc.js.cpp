// Copyright 2017 The Shaderc Authors. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "include/shaderc/shaderc.hpp"
#include <emscripten/bind.h>

using namespace emscripten;
using namespace shaderc;

EMSCRIPTEN_BINDINGS(shaderc) {
    class_<SpvCompilationResult>("SpvCompilationResult")
        .function("GetErrorMessage", &SpvCompilationResult::GetErrorMessage)
        .function("GetBinary",
            optional_override([](SpvCompilationResult& self) {
                size_t length = self.end() - self.begin();
                return val(typed_memory_view(length, self.begin()));
            }))
        ;

    class_<CompileOptions>("CompileOptions")
        .constructor<>()
        ;

    class_<Compiler>("Compiler")
        .constructor<>()
        .function("IsValid", &Compiler::IsValid)
        //.function("CompileGlslToSpv", &Compiler::CompileGlslToSpv)
        .function("CompileGlslToSpv",
            optional_override([](Compiler& self,
                const std::string& source_text,
                shaderc_shader_kind shader_kind,
                const std::string& input_file_name,
                const std::string& entry_point_name,
                const CompileOptions& options) {
                return self.CompileGlslToSpv(
                    source_text.c_str(),
                    source_text.length(),
                    shader_kind,
                    input_file_name.c_str(),
                    entry_point_name.c_str(),
                    options);
            }))
        ;

    enum_<shaderc_shader_kind>("shader_kind")
        .value("vertex", shaderc_vertex_shader)
        .value("fragment", shaderc_fragment_shader)
        .value("compute", shaderc_compute_shader)
        ;
}
