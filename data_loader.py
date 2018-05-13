import h5py
from Analysis import *
import os

#For deep learning
class Database_processing:
    def __init__(self,Database_Label,Dir_List):
        self.Database_Label = Data_Label #string
        self.Dir_List = Dir_List #list
        self.DataBox = [] #list
    
    def Create_New_Database(self,data):
        self.Save_file = h5py.File(self.Database_Label + ".h5", "w")
        self.Save_file.create_dataset(self.Database_Label, data = data)
    
    def Load_CSI(self,Dir_name):
        file_list = os.listdir(Dir_name)
        #Muilt file
        for filename in file_list:
            #Check direction
            if(os.path.isfile==False)
                continue
            #Check quantity
            Packet_quan = Check_Packet_Count(filename)
            Muilt_CSI(L_Range,H_Rang)

        
            


