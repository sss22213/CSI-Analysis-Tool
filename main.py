from subprocess import check_output
import numpy as np
class CSI_get:
    def __init__(self,path):
        self.Path = "\""+path+"\""

    def Get_Bfee_count(self,Packet_Number):
        cmd = "main "+self.Path+" "+str(Packet_Number)+" Bfee_count"
        return int(check_output(cmd, shell=True))

    def Get_Perm(self,Packet_Number):
        cmd = "main "+self.Path+" "+str(Packet_Number)+" Perm"
        result = str(check_output(cmd, shell=True))
        return [int(result[2]),int(result[4]),int(result[6])]

    def Get_Nrx(self,Packet_Number):
        cmd = "main "+self.Path+" "+str(Packet_Number)+" Nrx"
        return int(check_output(cmd, shell=True))

    def Get_Ntx(self,Packet_Number):
        cmd = "main "+self.Path+" "+str(Packet_Number)+" Ntx"
        return int(check_output(cmd, shell=True))

    def Get_Noise(self,Packet_Number):
        cmd = "main "+self.Path+" "+str(Packet_Number)+" Noise"
        return int(check_output(cmd, shell=True))

    def Get_RSSI(self,Packet_Number):
        cmd = "main "+self.Path+" "+str(Packet_Number)+" RSSI"
        result = str(check_output(cmd, shell=True))
        pos1 = result.find(',')
        pos2 = result.find(',', pos1+1)
        pos3 = result.find(',', pos2+1)
        return [int(result[2:pos1]),int(result[pos1+1:pos2]),int(result[pos2+1:pos3])]

    def Get_CSI(self,Packet_Number):
        cmd = "main "+self.Path+" "+str(Packet_Number)+" CSI"
        result = str(check_output(cmd, shell=True))
        pos1 = 0
        pos2 = 0
        pos3 = 0
        buff = []
        buff2 = []
        buff3 = []
        CSI_Box = []
        for index in range(180):
            pos3 = result.find('|',pos3+1)
            pos2 = result.find(',',pos2+1)
            pos1 = result.find('$',pos1+1)
            buff.append(pos1)
            buff2.append(pos2)
            buff3.append(pos3)
        count = 0
        for index in buff:
            prv = buff[count]
            mid = buff2[count]
            tail = buff3[count]
            count = count + 1
            CSI_Box.append(complex(int(result[prv+1:mid]),int(result[mid+1:tail])))
        return CSI_Box
    
    def Complete_Format(self,Packet_Number):
        self.Bfee_count = self.Get_Bfee_count(Packet_Number)
        self.Perm = self.Get_Perm(Packet_Number)
        self.Nrx = self.Get_Nrx(Packet_Number)
        self.Ntx = self.Get_Ntx(Packet_Number)
        self.Noise = self.Get_Noise(Packet_Number)
        self.RSSI = self.Get_RSSI(Packet_Number)
        self.CSI = np.sort_complex(np.zeros((30,3,2)))
        #self.CSI = np.complex(self.CSI)
        CSI_Packet = self.Get_CSI(Packet_Number)
        count = 0
        for Subcarrier in range(30):
            for Nrx in range(3):
                for Ntx in range(2):
                    self.CSI[Subcarrier,Nrx,Ntx] = CSI_Packet[count]
                    count = count + 1
        
CSI = CSI_get("<PATH>")
print(CSI.Complete_Format(5))
