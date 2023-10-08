root_dir="$(find ~ -type d -name railroad_ink)"
board_dir=$root_dir/railroad_board

alias inkpile="$root_dir/utils/inkpile.sh"
alias run_cut="$root_dir/cut/run_cut.sh -a \"$board_dir/board/AOS/*.c $board_dir/data/*.c $board_dir/utils/*.c\""

