#!/bin/bash
# Copyright (c) 2023, NVIDIA CORPORATION.

set -euo pipefail

package_name="pylibwholegraph"
package_dir="python/pylibwholegraph"

source rapids-configure-sccache
source rapids-date-string

# Use gha-tools rapids-pip-wheel-version to generate wheel version then
# update the necessary files
version_override="$(rapids-pip-wheel-version ${RAPIDS_DATE_STRING})"

RAPIDS_PY_CUDA_SUFFIX="$(rapids-wheel-ctk-name-gen ${RAPIDS_CUDA_VERSION})"

# This is the version of the suffix with a preceding hyphen. It's used
# everywhere except in the final wheel name.
PACKAGE_CUDA_SUFFIX="-${RAPIDS_PY_CUDA_SUFFIX}"

# Patch project metadata files to include the CUDA version suffix and version override.
pyproject_file="${package_dir}/pyproject.toml"

sed -i "s/^version = .*/version = \"${version_override}\"/g" ${pyproject_file}
sed -i "s/name = \"${package_name}\"/name = \"${package_name}${PACKAGE_CUDA_SUFFIX}\"/g" ${pyproject_file}

# For nightlies we want to ensure that we're pulling in alphas as well. The
# easiest way to do so is to augment the spec with a constraint containing a
# min alpha version that doesn't affect the version bounds but does allow usage
# of alpha versions for that dependency without --pre
alpha_spec=''
if ! rapids-is-release-build; then
    alpha_spec=',>=0.0.0a0'
fi

cd "${package_dir}"

# Hardcode the output dir
SKBUILD_CONFIGURE_OPTIONS="-DDETECT_CONDA_ENV=OFF -DBUILD_SHARED_LIBS=OFF -DCMAKE_MESSAGE_LOG_LEVEL=VERBOSE -DCUDA_STATIC_RUNTIME=ON -DWHOLEGRAPH_BUILD_WHEELS=ON" \
  python -m pip wheel . -w dist -vvv --no-deps --disable-pip-version-check

mkdir -p final_dist
python -m auditwheel repair --exclude libcuda.so.1 -w final_dist dist/*

RAPIDS_PY_WHEEL_NAME="${package_name}_${RAPIDS_PY_CUDA_SUFFIX}" rapids-upload-wheels-to-s3 final_dist
