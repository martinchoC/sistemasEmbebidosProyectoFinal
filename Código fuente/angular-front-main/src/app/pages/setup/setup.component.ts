import { Component, OnInit } from '@angular/core';
import { FormControl, FormGroup, Validators } from '@angular/forms';
import { Parametros } from 'src/app/model/parametros';
import { ParametrosService } from 'src/app/services/parametros.service';

@Component({
  selector: 'app-setup',
  templateUrl: './setup.component.html',
  styleUrls: ['./setup.component.css']
})
export class SetupComponent implements OnInit {

  formulario:FormGroup;

  constructor(private parametrosService:ParametrosService) { }

  ngOnInit(): void {
    this.formulario = new FormGroup({
      humedadAmbienteMinima: new FormControl("",Validators.required),
      humedadSueloMaxima: new FormControl("",Validators.required),
      humedadSueloMinima: new FormControl("",Validators.required),
      temperaturaMaxima: new FormControl("",Validators.required),
    })
    this.parametrosService.getParametro().subscribe(response =>{
      this.formulario.controls['humedadAmbienteMinima'].setValue(response.payload.val().humedad_ambiente_minima);
      this.formulario.controls['humedadSueloMaxima'].setValue(response.payload.val().humedad_suelo_maxima);
      this.formulario.controls['humedadSueloMinima'].setValue(response.payload.val().humedad_suelo_minima);
      this.formulario.controls['temperaturaMaxima'].setValue(response.payload.val().temperatura_maxima);
    })
  }

  onSubmit()
  {
      let salida:Parametros = {
        humedad_ambiente_minima: this.formulario.value.humedadAmbienteMinima,
        humedad_suelo_minima: this.formulario.value.humedadSueloMinima,
        humedad_suelo_maxima: this.formulario.value.humedadSueloMaxima,
        temperatura_maxima: this.formulario.value.temperaturaMaxima
      }
      this.parametrosService.updateParametros(salida);
  }

}
