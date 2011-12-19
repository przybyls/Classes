#
# Copyright 2011 Leo Przybylski
#
# Educational Community License, Version 2.0 (the "License"); you may
# not use this file except in compliance with the License. You may
# obtain a copy of the License at
#
# http://www.osedu.org/licenses/ECL-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an "AS IS"
# BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
# or implied. See the License for the specific language governing
# permissions and limitations under the License.
#

# Make stuff. All purpose compile

# Builds prog10 binary in $PWD
prog10: compile
	cp src/prog10 .

# Runs configuration to properly build the application
configure: 
	aclocal
	automake --add-missing
	automake
	autoconf
	./configure

# Compiles all known source files
compile: configure
	make -C src

# Builds all programs
all: prog10

# Cleanup or startover
clean:
	make -C src clean
	rm -rf Makefile.in  aclocal.m4  autom4te.cache/ config.* autom4te.cache configure