#!/bin/sh

command="modules/create/.patch"
perl $command && export SECURE_MISC="no" || export SECURE_MISC="yes"
