import { Component, OnInit } from '@angular/core';
import { Data } from 'src/app/model/data';
import { SavesService } from 'src/app/services/saves.service';

@Component({
  selector: 'app-saves',
  templateUrl: './saves.component.html',
  styleUrls: ['./saves.component.css']
})
export class SavesComponent implements OnInit {

  datos:Data[] = [];

  constructor(private savesService:SavesService) { }

  ngOnInit(): void {
    this.savesService.getSaves().subscribe(response =>{
      response.forEach(element => this.datos.push(<Data>element.payload.val()));
      console.log(this.datos);
    })
  }

}
