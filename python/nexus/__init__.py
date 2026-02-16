from __future__ import annotations

from . import _core as _core

# Public submodules (available immediately if bound)
math = _core.math

# Convenience aliases (stable)
Vector = math.Vector

__all__ = [
    "_core",
    "math",
    "Vector",
]
