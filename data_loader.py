import h5py
from Analysis import *

#For deep learning
class Database_processing:
    def __init__(self,Database_Label,CSI_Group_file):
        self.Database_Label = Data_Label #string
        self.CSI = CSI_Group_file #list
        self.DataBox = [] #list
    
    def Create_New_Database(self,data):
        self.Save_file = h5py.File(self.Database_Label + ".h5", "w")
        self.Save_file.create_dataset(self.Database_Label, data = data)
    
    #def Load_CSI(self,num = None):
            


