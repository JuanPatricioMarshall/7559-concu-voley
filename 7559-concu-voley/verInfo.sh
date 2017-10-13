kill -10 $(ps aux | grep 'Concu' | awk '{print $2}' | head -1)
