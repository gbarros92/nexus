#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <string>

#include "math/vector.hpp"

namespace py = pybind11;

namespace nexus {

void bind_math(py::module_& m) {
    // Create submodule: _core.math
    py::module_ mathm = m.def_submodule("math", "Math types and operations");

    using Vec = ::nexus::math::Vector<double>;

    py::class_<Vec>(mathm, "Vector")
        .def(py::init<unsigned int>(), py::arg("dim"))
        .def(py::init<unsigned int, const double&>(), py::arg("dim"), py::arg("value"))
        .def(py::init<std::initializer_list<double>>(), py::arg("values"))

        // Python protocol helpers
        .def("__len__", &Vec::dim)
        .def("dim", &Vec::dim)

        // Element access (add bounds checks at Python boundary)
        .def("__getitem__", [](const Vec& v, size_t i) -> double {
            if (i >= static_cast<size_t>(v.dim()))
                throw py::index_error("Vector index out of range");
            return v[static_cast<unsigned int>(i)];
        })
        .def("__setitem__", [](Vec& v, size_t i, double val) {
            if (i >= static_cast<size_t>(v.dim()))
                throw py::index_error("Vector index out of range");
            v[static_cast<unsigned int>(i)] = val;
        })

        // Operators (these call your C++ operators)
        .def(py::self + py::self)
        .def(py::self - py::self)
        .def(py::self * double())
        .def(py::self / double())
        .def(double() * py::self)

        .def("__repr__", [](const Vec& v) {
            return "<nexus.math.Vector dim=" + std::to_string(v.dim()) + ">";
        });
}

} // namespace nexus

PYBIND11_MODULE(_core, m) {
    m.doc() = "Nexus C++ core bindings";
    nexus::bind_math(m);
}
