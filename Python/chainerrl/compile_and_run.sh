g++ -shared -fPIC -I../../Field -L../../Field -I../../GameManager -L../../GameManager -lpython3.6m -I/usr/include/python3.6m -lboost_numpy3 -lboost_python3 -o communication.so ../../Field/field.cpp ../../GameManager/GameManager.cpp ../communication.cpp
# LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/home/shibh308/workspace/procon2018/procon2018-comp/Field" 
python main.py
