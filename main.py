from subprocess import check_output
import numpy as np
import matplotlib.pyplot as plt

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
        if(len(result) < self.Get_Nrx(Packet_Number)*self.Get_Ntx(Packet_Number)*30):
            return "CSI size is error"
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
        Bfee_count = self.Get_Bfee_count(Packet_Number)
        Perm = self.Get_Perm(Packet_Number)
        Nrx = self.Get_Nrx(Packet_Number)
        Ntx = self.Get_Ntx(Packet_Number)
        Noise = self.Get_Noise(Packet_Number)
        RSSI = self.Get_RSSI(Packet_Number)
        CSI = np.sort_complex(np.zeros((30,3,2)))
        #self.CSI = np.complex(self.CSI)
        CSI_Packet = self.Get_CSI(Packet_Number)
        if(len(CSI_Packet) < 20):
            return CSI_Packet
        count = 0
        for Subcarrier in range(30):
            for Nrx in range(3):
                for Ntx in range(2):
                    CSI[Subcarrier,Nrx,Ntx] = CSI_Packet[count]
                    count = count + 1
        return [Bfee_count,Perm,Nrx,Ntx,Noise,RSSI,CSI]

    def Muilt_data(self,L_Range,H_Rang):
        Box = []
        buff = 0
        for Packet_number in range(H_Rang-L_Range):
            if(self.Get_Bfee_count(Packet_number) != buff):
                Box.append(self.Complete_Format(Packet_number))
                buff = self.Get_Bfee_count(Packet_number)
        return Box

CSI = CSI_get("D:\\octave\\170109_2432st_09.dat")
time = []
point1 = []
point2 = []
point3 = []
point4 = []
point5 = []
point6 = []
CSI_result = CSI.Get_CSI(1)

for subcarrier in range(0,180,6):
    point1.append(abs(CSI_result[subcarrier]))
    point2.append(abs(CSI_result[subcarrier+1]))
    point3.append(abs(CSI_result[subcarrier+2]))
    point4.append(abs(CSI_result[subcarrier+3]))
    point5.append(abs(CSI_result[subcarrier+4]))
    point6.append(abs(CSI_result[subcarrier+5]))

for time_index in range(30):
    time.append(time_index)

plt.axis([0, 30, 0, 60])
plt.xlabel('subcarrier')
plt.ylabel('amplitude(db)') 
plt.plot(time,point1,'b')
plt.plot(time,point2,'b')
plt.plot(time,point3,'g')
plt.plot(time,point4,'g')
plt.plot(time,point5,'r')
plt.plot(time,point6,'r')
plt.show()
