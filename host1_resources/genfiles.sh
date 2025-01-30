yes 1 | tr -d '\n' | head -c 65250 > too_large
yes 1 | tr -d '\n' | head -c 65249 > max_size
