# Experimental Area

This directory is reserved for temporary prototypes, demos, and migration spikes.

Rules:
- No default CMake targets may depend on files here.
- Any code moved from here to production must be reviewed and integrated in small patches.
- Temporary behavior must be tagged with `TEMPORARY(...)` marker in code comments.
