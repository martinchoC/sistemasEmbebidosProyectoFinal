import { Component, OnInit } from '@angular/core';
import { SnapshotAction, snapshotChanges } from '@angular/fire/database';
import { Data } from 'src/app/model/data';
import { DataService } from 'src/app/services/data.service';
import { SavesService } from 'src/app/services/saves.service';

@Component({
  selector: 'app-index',
  templateUrl: './index.component.html',
  styleUrls:['./index.component.css']
})
export class IndexComponent implements OnInit {

  dataSnapshot:any;
  constructor(private dataService:DataService, private saveService:SavesService) { }

  ngOnInit(): void {
   this.dataService.getData().subscribe(response =>{
    this.dataSnapshot = response.payload.val();
   })
  }

  saveData()
  {
    this.saveService.postSave(this.dataSnapshot);
  }

}
