import torch
import torch.nn as nn
import numpy as np

#data
x = np.array([[0,0],[1,1],[1,0],[0,1]])
y = np.array([[0],[0],[1],[1]])
x = torch.Tensor(x).float()
y = torch.Tensor(y).float()

#network design
class XOR(nn.Module):
    def __init__(self):
        super(XOR,self).__init__()
        self.input_layer = nn.Linear(2,2)
        self.relu = nn.ReLU()
        self.output_layer = nn.Linear(2,1)

    def forward(self,x):
        o1 = self.relu(self.input_layer(x))
        o2 = self.relu(self.output_layer(o1))
        return o2

xor = XOR()
loss_function = nn.L1Loss()
optimizer = torch.optim.SGD(xor.parameters(), lr=1e-3, momentum=0.9)

#train
for epoch in range(5000):
    out = xor(x)
    loss = loss_function(out,y)
    optimizer.zero_grad()
    loss.backward()
    optimizer.step()

#test
out = xor(x)
print(out)