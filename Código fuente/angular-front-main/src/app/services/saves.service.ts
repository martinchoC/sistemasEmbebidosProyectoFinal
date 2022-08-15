import { Injectable } from '@angular/core';
import { AngularFireDatabase, AngularFireList, SnapshotAction } from '@angular/fire/database';
import { Data } from '@angular/router';
import { Observable } from 'rxjs';

@Injectable({
  providedIn: 'root'
})
export class SavesService {

  dbSaves:AngularFireList<Data>;

  constructor(private db:AngularFireDatabase) {
    this.dbSaves = db.list("/saves");
   }

   getSaves():Observable<SnapshotAction<Data>[]>
   {
     return this.dbSaves.snapshotChanges()
   }

   postSave(data:Data)
   {
     this.dbSaves.push(data);
   }
}
