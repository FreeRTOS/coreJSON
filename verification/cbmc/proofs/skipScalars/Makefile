# Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
# SPDX-License-Identifier: MIT-0

PROOF_UID=skipScalars
CHECK_FUNCTION_CONTRACTS = $(PROOF_UID)
HARNESS_FILE= $(PROOF_UID)_harness

CBMC_OBJECT_BITS = 12

# This value was experimentally chosen to provide 100% coverage
# without tripping unwinding assertions and without exhausting memory.
CBMC_MAX_BUFSIZE=10

UNWINDSET += skipArrayScalars.0:$(CBMC_MAX_BUFSIZE)
UNWINDSET += skipObjectScalars.0:$(CBMC_MAX_BUFSIZE)

USE_FUNCTION_CONTRACTS += skipSpace
USE_FUNCTION_CONTRACTS += skipAnyScalar
USE_FUNCTION_CONTRACTS += skipString

include ../Makefile-json.common

# Substitution command to pass to sed for patching core_json.c. The
# characters " and # must be escaped with backslash.
CORE_JSON_SED_EXPR = s/^static //
