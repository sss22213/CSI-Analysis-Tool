import h5py
from Analysis import *
import os

#For deep learning
class Database_processing:
    def __init__(self,Database_Label,Dir_List):
        self.Database_Label = Database_Label #string
        self.Dir_List = Dir_List #list
        self.DataBox = [] #list
    
    def Create_New_Database(self,data):
        self.Save_file = h5py.File(self.Database_Label + ".h5", "w")
        self.Save_file.create_dataset(self.Database_Label, data = data)
    
    def Load_CSI(self,Dir_name):
        file_list = os.listdir(Dir_name)
        Box = []
        #Muilt file
        for filename in file_list:
            '''
            #Check direction
            if(os.path.isfile==False):
                continue
            '''
            CSI = CSI_get(Dir_name+'\\'+filename)
            #Check quantity
            Packet_quan = CSI.Check_Effection_Packet()
            print(Dir_name+'\\'+filename)
            for CSI_number in Packet_quan:
                Box.append(CSI.Get_CSI(CSI_number))
        return np.array(Box)

if __name__ == '__main__':
    DP = Database_processing("SS",['F:'])
    DP.Load_CSI("F:\\location2\\1")


                
                
            

        
            


