while IFS= read -r inp || [ -n "$inp" ]; do
  printf '%s_$\n' "$inp"
done
