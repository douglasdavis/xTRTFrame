export xTRTFrame_SRC="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

lsetup 'sft releases/LCG_87/yamlcpp/0.5.1'
export YAMLCPP_ROOT=$SFT_HOME_yamlcpp

alias cmake_xtrtframe='cd $xTRTFrame_SRC/../build; cmake -DYAMLCPP_ROOT=$YAMLCPP_ROOT ../xTRTFrame'
