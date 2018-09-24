import h5py
from Analysis import *
import os

#For deep learning
class Database_processing:
    def __init__(self,Database_Name,Database_Label,Dir_List):
        self.Database_Name = Database_Name #string
        self.Database_Label = Database_Label #list
        self.Dir_List = Dir_List #list
        self.DataBox = [] #list
    
    def Create_New_Database(self):
        count = 0
        f = h5py.File(self.Database_Name + ".h5", "w")
        for dirname_dir in self.Dir_List:
            data = self.Load_CSI(dirname_dir)
            f.create_dataset(self.Database_Label[count],data=data)
            count = count + 1
        f.flush
        f.close
        f.close

    def Create_New_Database_CSI_phase(self):
        count = 0
        f = h5py.File(self.Database_Name + ".h5", "w")
        for dirname_dir in self.Dir_List:
            data = self.Load_CSI_phase(dirname_dir)
            f.create_dataset(self.Database_Label[count],data=data)
            count = count + 1
        f.flush
        f.close
        f.close

    def Create_New_Database_RSSI(self):
        count = 0
        f = h5py.File(self.Database_Name + ".h5", "w")
        for dirname_dir in self.Dir_List:
            data = self.Load_RSSI(dirname_dir)
            f.create_dataset(self.Database_Label[count],data=data)
            count = count + 1
        f.flush
        f.close
        f.close

    def Load_CSI(self,Dir_name):
        file_list = os.listdir(Dir_name)
        Box = []
        #Muilt file
        for filename in file_list:
            #Check direction
            if(os.path.isfile==False):
                continue
            CSI = CSI_get(Dir_name+'\\'+filename)
            #Check quantity
            Packet_quan = CSI.Check_Effection_Packet()
            print(Dir_name+'\\'+filename)
            for CSI_number in Packet_quan:
                try:
                    result = CSI.Get_CSI_Q(CSI_number)
                    if(type(result)==type(1)):
                        continue
                    Box.append(result)
                except:
                    continue
        return Box

    def Load_CSI_phase(self,Dir_name):
        file_list = os.listdir(Dir_name)
        Box = []
        #Muilt file
        for filename in file_list:
            #Check direction
            if(os.path.isfile==False):
                continue
            CSI = CSI_get(Dir_name+'\\'+filename)
            #Check quantity
            Packet_quan = CSI.Check_Effection_Packet()
            print(Dir_name+'\\'+filename)
            for CSI_number in Packet_quan:
                try:
                    result = CSI.Get_CSI_Ang(CSI_number)
                    if(type(result)==type(1)):
                        continue
                    Box.append(result)
                except:
                    continue
        return Box

    def Load_RSSI(self,Dir_name):
        file_list = os.listdir(Dir_name)
        Box = []
        print("Initial..")
        #Muilt file
        for filename in file_list:
            #Check direction
            if(os.path.isfile==False):
                continue
            RSSI = CSI_get(Dir_name+'\\'+filename)
            #Check quantity
            Packet_quan = RSSI.Check_Effection_Packet()
            print(Dir_name+'\\'+filename)
            for CSI_number in Packet_quan:
                try:
                    result = RSSI.Get_RSSI(CSI_number)
                    if(type(result)==type(1)):
                        continue
                    Box.append(result)
                except:
                    continue
        return Box

if __name__ == '__main__':
    '''
    Create Database_processing object
    Argumnet: 
        Database_Name: Database's name->string, 
        Database_Label: Every h5's name->list,
        Dir_List: where is dat file->list,

        File structure example:
        root dirname
        -1
            --  one.dat
            --  two.dat
        -2
            --  one.dat
            --  two.dat
    '''
    label = []
    dirlist = []
    for label_index in range(1,2):
        label.append(str(label_index))
        dirlist.append("Z:\\root dirname\\"+str(label_index))
    DP = Database_processing("CSI",label,dirlist)
    DP.Create_New_Database()
    